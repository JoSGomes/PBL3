@extends('layouts.template')

@section('title')
    Home
@endsection

@section('body')
    <div class="subtitle">Dispositivo [ESTADO]</div>
    <div class="subtitle">Contatos de emergência</div>
    
    <table class="table table-hover table-bordeless">
        <thead>
        <tr>
            <th scope="col">#</th>
            <th scope="col">First</th>
            <th scope="col">Last</th>
            <th scope="col">Handle</th>
        </tr>
        </thead>
        <tbody>
        <tr >
            <th scope="row">1</th>
            <td>Mark</td>
            <td>Otto</td>
            <td>@mdo</td>
        </tr>
        </tbody>
    </table>
@endsection